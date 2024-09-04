SELECT 
  list_aggregate(tau_pt, 'array_agg'),
  list_aggregate(tau_eta, 'array_agg'),
  list_aggregate(jet_pt, 'array_agg'),
  list_aggregate(jet_eta, 'array_agg'),
  list_aggregate(muon_pt, 'array_agg'),
  list_aggregate(muon_eta, 'array_agg'),
  list_aggregate(el_pt, 'array_agg'),
  list_aggregate(el_eta, 'array_agg'),
  list_aggregate(ph_pt, 'array_agg'),
  list_aggregate(ph_eta, 'array_agg')  
FROM singleMu;